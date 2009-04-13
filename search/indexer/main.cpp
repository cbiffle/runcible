#include <CLucene/StdHeader.h>
#include <CLucene.h>

#include <QtCore>

#include <proto/indexer.pb.h>

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;

using std::auto_ptr;

using runcible::search::IndexItem;

/*
 * Converts an STL string to an array of Lucene's TCHARs.
 * Note that this conversion is ALMOST CERTAINLY WRONG
 * for strings containing anything other than 7-bit ASCII.
 *
 * The returned pointer needs to be delete[]d.
 */
const TCHAR* toTchar(const std::string &str) {
  TCHAR* text = new TCHAR[str.size() + 1];
  STRCPY_AtoT(text, str.c_str(), str.size() + 1);
  return text;
}

/*
 * Converts an IndexItem proto into a Lucene Document.
 */
auto_ptr<Document> doc_from_proto(const IndexItem &proto) {
  auto_ptr<Document> doc( new Document() );

  const TCHAR *urlText = toTchar(proto.url());
  doc->add(*new Field(_T("url"), urlText, Field::STORE_YES | Field::INDEX_UNTOKENIZED));
  delete[] urlText;

  const TCHAR *titleText = toTchar(proto.title());
  doc->add(*new Field(_T("title"), titleText, Field::STORE_YES | Field::INDEX_UNTOKENIZED));
  delete[] titleText;

  const TCHAR *contentsText = toTchar(proto.plain_contents());
  doc->add(*new Field(_T("contents"), contentsText, Field::STORE_NO | Field::INDEX_TOKENIZED));
  delete[] contentsText;

  return doc;
}

void index_dir(IndexWriter *writer, const QString &path) {
  QDirIterator dir(path, QDirIterator::Subdirectories);
  while (dir.hasNext()) {
    dir.next();
    QFileInfo fileInfo(dir.filePath());
    if (!fileInfo.isFile()) continue;

    QString binaryName("runcible-index-ext-" + fileInfo.suffix().toLower());

    QProcess proc;
    proc.start(binaryName, QStringList() << QUrl::fromLocalFile(dir.filePath()).toString());
    if (!proc.waitForStarted()) { 
      qDebug() << "Cannot start indexer for" << dir.filePath();
      continue;
    }
    proc.waitForFinished(); // doesn't seem to work without this :-(

    QDataStream procOut(&proc);

    QByteArray bytes;
    IndexItem msg;
    while (proc.bytesAvailable() >= sizeof(quint32)) {
      procOut >> bytes;
      if (bytes.isEmpty()) continue;
      msg.ParseFromArray(bytes.constData(), bytes.size());
      auto_ptr<Document> doc( doc_from_proto(msg) );
      writer->addDocument(doc.get());
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: runcible-search-index <docroot> <indexroot>\n");
    return 1;
  }

  const QString docRoot(argv[1]);
  const QString indexRoot(argv[2]);

  const QByteArray indexRootAscii = indexRoot.toAscii();
  const char *indexRootChars = indexRootAscii.constData();

  IndexWriter *writer = NULL;
  standard::StandardAnalyzer analyzer;

  if (!true && IndexReader::indexExists(indexRootChars)) {
    if (IndexReader::isLocked(indexRootChars)) {
      qDebug() << "Index was locked.";
      IndexReader::unlock(indexRootChars);
    }
    writer = new IndexWriter(indexRootChars, &analyzer, false);
  } else {
    writer = new IndexWriter(indexRootChars, &analyzer, true);
  }

  writer->setMaxFieldLength(IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

  index_dir(writer, docRoot);

  writer->optimize();
  writer->close();
  delete writer;
}
